use Time::HiRes qw/time/;
if(/(\d+\D){3}/){
	$t2 = time();
	$i++;
	($d,$c,$m)=split /\s/s;
	$t=$d+$m+$c;
	last if$t==0;
	print"\n"if$i!=1;
	print"Data set $i:\n";
	@d=();
	while($t--){
		$_=<>;
		chomp;
		push@d,[split / /]
	}
	@m=splice@d,$d+$c;
	@c=splice@d,$d;
	$t1 = time();
	print STDERR "Time in: ".($t1-$t2)."\n";
	$t2 = $t1;
	&m(\@m,\@d,1);
	$t1 = time();
	print STDERR "Time do: ".($t1-$t2)."\n";
	$t2 = $t1;
	&m(\@m,\@c,2);
	$t1 = time();
	print STDERR "Time ca: ".($t1-$t2)."\n";
	$t2 = $t1;
	@s=qw/neither safe eaten/;
	print"Mouse at ($_->[0],$_->[1]) is $s[$_->[3]]\n"for(@m);
	$t1 = time();
	print STDERR "Time out: ".($t1-$t2)."\n";
	$t2 = $t1;
#	print "$d $c $m\n";
#	print "$_->[0] $_->[1]\n" for(@d,@c);
#	print "$_->[0] $_->[1] $s[$_->[3]]\n" for(@m);
}
sub m{
	my@m=@{$_[0]};
	@t=@{$_[1]};
	if(@t>2){
		($x,$y)=(($t[0][0]+$t[1][0]+$t[2][0])/3,($t[0][1]+$t[1][1]+$t[2][1])/3)
	}elsif(@t==2){
		($x,$y)=(($t[0][0]+$t[1][0])/2,($t[0][1]+$t[1][1])/2)
	}elsif(@t==1){
		for(@m){
			$_->[3]=$_[2]if$_->[0]==$t[0][0]&&$_->[1]==$t[0][1]
		}
		return
	}
	else{
		return
	}
	$_->[2]=atan2($_->[1]-$y,$_->[0]-$x)for(@t,@m);
	$s=sub {
		$a->[2]<=>$b->[2]
	};
	@t=sort{$s->()}@t;
	for($t=0;$t<@t;$t++){
		if($t[$t][0]==$x&&$t[$t][1]==$y&&@t>1){
			splice@t,$t,1;
			$t--
		}

	}
#	print "Centre: $x,$y\n";
	for($t=0;$t<$#t;$t++){
		if($t[$t][2]==$t[$t+1][2]){
			if($t[$t][0]*$t[$t][0]+$t[$t][1]*$t[$t][1]>=$t[$t+1][0]*$t[$t+1][0]+$t[$t+1][1]*$t[$t+1][1]){
				splice@t,$t+1,1;
				$t--
			}
		}
	}
	if(@t==1){
		for(@m){
			$_->[3]=$_[2]if$_->[0]==$t[0][0]&&$_->[1]==$t[0][1]
		}
		return
	}
	elsif(@t==0){
		return
	}
	for($t=0;$t<@t;$t++){
		$p=($t-1)%@t;
		$n=($t+1)%@t;
		($dx1,$dx2,$dy1,$dy2)=($t[$t][0]-$t[$p][0],$t[$n][0]-$t[$t][0],$t[$t][1]-$t[$p][1],$t[$n][1]-$t[$t][1]);
		$sinsign=$dy1*$dx2-$dx1*$dy2;
		if($sinsign>0){
			splice(@t,$t,1);
			$t=0;
			#$t=0 if$t<0
		}
	}
#	print scalar(@t)."\n";
	for(@t){
#		print"Remaining: $_->[0], $_->[1], $_->[2]\n";
	}
	$n=0;
	$_->[2]-=atan2(0,-1)*2 for grep{
		$_->[2]>$t[$#t][2]
	}
	@m;
	@m=sort{$s->()}@m;
	for(my$m=0;$m<@m;$m++){
		next if$m[$m][3];
		$n++while$m[$m][2]>$t[$n][2]&&$n<$#t;
		if($m[$m][2]==$t[$n][2]){
			($mx,$my,$tx,$ty)=($m[$m][0]-$x,$m[$m][1]-$y,$t[$n][0]-$x,$t[$n][1]-$y);
			$m[$m][3]=$_[2]if $mx*$mx+$my*$my<=$tx*$tx+$ty*$ty
		}
		else{
			$t=($n-1)%@t;
			($dx1,$dx2,$dy1,$dy2)=($m[$m][0]-$t[$t][0],$t[$n][0]-$m[$m][0],$m[$m][1]-$t[$t][1],$t[$n][1]-$m[$m][1]);
			$sinsign=$dy1*$dx2-$dx1*$dy2;
			$m[$m][3]=$_[2]if($sinsign>=0)
		}

	}

}

